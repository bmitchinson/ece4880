import { h, Component } from 'preact';
import { Card } from 'preact-fluid';

import { auth, googleAuthProvider } from '../firebase/firebase';

import Temp from '../temp/temp';
import Graph from '../graph/graph';
import Controls from '../controls/controls';

import style from './componentGrid.scss';

interface MyProps {}
interface MyState {
    currentUser: any;
}

export default class ComponentGrid extends Component<MyProps, MyState> {
    constructor() {
        super();
        this.state = {
            currentUser: null
        };
    }

    componentDidMount() {
        auth.onAuthStateChanged(currentUser => {
            if (currentUser) {
                this.setState({ currentUser }, () => console.log('Logged In!'));
            } else {
                this.setState({ currentUser }, () => console.log('Logged Out!'));
            }
        });
    }

    render() {
        const user = this.state.currentUser;
        const name = user ? user.displayName : 'No one';
        return (
            <div>
                <button onClick={() => auth.signInWithRedirect(googleAuthProvider)}>Sign In</button>
                {user && <p>Signed In As: {name}</p>}
                <button onClick={() => auth.signOut()}>Sign Out</button>
                <div class={style.Grid}>
                    <Card className={style.TempCard}>
                        <Temp />
                    </Card>
                    <Card className={style.GraphCard}>
                        <Graph />
                    </Card>
                    <Card className={style.ControlsCard}>
                        <Controls />
                    </Card>
                </div>
            </div>
        );
    }
}
